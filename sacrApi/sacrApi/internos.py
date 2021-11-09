def json_response(s):
    import json
    return json.dumps(s, default=str)